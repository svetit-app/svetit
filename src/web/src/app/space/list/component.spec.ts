import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { SpaceListComponent } from './component';

describe('SpaceListComponent', () => {
  let component: SpaceListComponent;
  let fixture: ComponentFixture<SpaceListComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
    imports: [SpaceListComponent]
})
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(SpaceListComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
