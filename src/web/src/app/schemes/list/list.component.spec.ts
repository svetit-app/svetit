import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { SchemeListComponent } from './list.component';

describe('SchemeListComponent', () => {
  let component: SchemeListComponent;
  let fixture: ComponentFixture<SchemeListComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
    imports: [SchemeListComponent]
})
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(SchemeListComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
