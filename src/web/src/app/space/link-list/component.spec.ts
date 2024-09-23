import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { SpaceLinkListComponent } from './component';

describe('SpaceLinkListComponent', () => {
  let component: SpaceLinkListComponent;
  let fixture: ComponentFixture<SpaceLinkListComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
    imports: [SpaceLinkListComponent]
})
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(SpaceLinkListComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
